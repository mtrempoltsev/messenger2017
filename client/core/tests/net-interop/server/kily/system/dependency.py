import queue
from typing import Callable, Iterable

from kily.system.core import Module, ModuleStatus


class UnresolvedDependencyError(RuntimeError):
    def __init__(self, mod: Module, dependency: str):
        super().__init__("Unresolved dependency '{}' for module: '{}'".format(dependency, mod.NAME))


class DependencyCycleError(RuntimeError):
    def __init__(self, mod: Module, involved_module_names: set):
        super().__init__("Dependency in module {} causes dependency cycle."
                         "Involved modules: {}".format(mod.NAME, involved_module_names))


class DependencyModuleTreeForest:
    def __init__(self):
            self._tree = {}

    @staticmethod
    def update_depth(node, diff):
        node[2] += diff
        for v in node[1].values():
            DependencyModuleTreeForest.update_depth(v, diff)

    @staticmethod
    def _reparent(dependency_name: str, new_parent_node, old_parent_node, resolved_dependencies: dict):
        dependency = old_parent_node[1][dependency_name]
        new_parent_node[1][dependency_name] = dependency
        resolved_dependencies[dependency_name] = new_parent_node
        del old_parent_node[1][dependency_name]
        DependencyModuleTreeForest.update_depth(dependency, new_parent_node[2]-old_parent_node[2])

    def _resolve(self, mod: Module, *modules,
                 resolved_dependencies: dict, recursion_elems: set, parent_node: list,
                 mod_deps: Callable[[Module], Iterable[str]]):
        node = [mod, {}, parent_node[2] + 1]
        if mod.status.value < ModuleStatus.LOADED.value:
            recursion_elems |= {mod.NAME}
            for ur in mod_deps(mod):
                if ur in recursion_elems:
                    raise DependencyCycleError(mod, recursion_elems)
                if resolved_dependencies.get(ur):
                    dependency_parent_node = resolved_dependencies[ur]
                    if dependency_parent_node[2] < node[2]:
                        DependencyModuleTreeForest._reparent(ur, node, dependency_parent_node, resolved_dependencies)
                else:
                    dep = next((m for m in modules if m.NAME == ur), None)
                    if dep:
                        assert isinstance(dep, Module)
                        self._resolve(dep, *modules,
                                      resolved_dependencies=resolved_dependencies,
                                      recursion_elems=recursion_elems, parent_node=node,
                                      mod_deps=mod_deps)
                        resolved_dependencies[ur] = node
                    else:
                        raise UnresolvedDependencyError(mod, ur)
            recursion_elems.remove(mod.NAME)
        # noinspection PyTypeChecker
        parent_node[1][mod.NAME] = node
        if parent_node[0] is None:
            resolved_dependencies[mod.NAME] = parent_node

    @classmethod
    def _default_filter(cls, mod: Module):
        return mod.status.value <= ModuleStatus.LOADED.value

    @classmethod
    def _default_deps(cls, mod: Module) -> Iterable[str]:
        return mod.DEPENDENCIES

    def build(self, *modules: 'Module',
              mod_filter: Callable[[Module], bool] = None,
              mod_deps: Callable[[Module], Iterable[str]] = None):
        # find first module to seek
        if not len(modules):
            return
        resolved_dependencies = dict()
        root_node = [None, self._tree, 0]
        mod_filter = mod_filter if mod_filter is not None else self._default_filter
        mod_deps = mod_deps if mod_deps is not None else self._default_deps
        for mod in modules:
            if mod_filter(mod) and mod.NAME not in resolved_dependencies.keys():
                self._resolve(mod, *modules, resolved_dependencies=resolved_dependencies,
                              recursion_elems=set(), parent_node=root_node, mod_deps=mod_deps)

    def to_queue(self):
        """LEVEL-ORDERED TREE-FOREST PRINT"""
        to_view = [v for v in self._tree.values()]
        q = queue.deque()
        while len(to_view):
            to_view += [v1 for v1 in to_view[0][1].values()]
            q.appendleft(to_view[0][0])
            del to_view[0]
        return q
